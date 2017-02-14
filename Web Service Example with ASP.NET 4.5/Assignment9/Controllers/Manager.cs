using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
// new...
using AutoMapper;
using Assignment9.Models;
using System.Security.Claims;

namespace Assignment9.Controllers
{
    public class Manager
    {
        // Reference to the data context
        private ApplicationDbContext ds = new ApplicationDbContext();

        // Get AutoMapper instance
        public IMapper mapper = AutoMapperConfig.RegisterMappings();

        // Declare a property to hold the user account for the current request
        // Can use this property here in the Manager class to control logic and flow
        // Can also use this property in a controller 
        // Can also use this property in a view; for best results, 
        // near the top of the view, add this statement:
        // var userAccount = new ConditionalMenu.Controllers.UserAccount(User as System.Security.Claims.ClaimsPrincipal);
        // Then, you can use "userAccount" anywhere in the view to render content
        public UserAccount UserAccount { get; private set; }

        public Manager()
        {
            // If necessary, add constructor code here

            // Initialize the UserAccount property
            UserAccount = new UserAccount(HttpContext.Current.User as ClaimsPrincipal);

            // Turn off the Entity Framework (EF) proxy creation features
            // We do NOT want the EF to track changes - we'll do that ourselves
            ds.Configuration.ProxyCreationEnabled = false;

            // Also, turn off lazy loading...
            // We want to retain control over fetching related objects
            ds.Configuration.LazyLoadingEnabled = false;
        }

        // Add methods below
        // Controllers will call these methods
        // Ensure that the methods accept and deliver ONLY view model objects and collections
        // The collection return type is almost always IEnumerable<T>

        // Suggested naming convention: Entity + task/action
        // For example:
        // ProductGetAll()
        // ProductGetById()
        // ProductAdd()
        // ProductEdit()
        // ProductDelete()






        // Add some programmatically-generated objects to the data store
        // Can write one method, or many methods - your decision
        // The important idea is that you check for existing data first
        // Call this method from a controller action/method
        // ####################################################################
        // ALBUM

            public IEnumerable<AlbumBase> AlbumGetAll()
        {
            return mapper.Map<IEnumerable<Album>, IEnumerable<AlbumBase>>(ds.Albums.OrderBy(t => t.Name));
        }

        public IEnumerable<AlbumWithDetails> AlbumGetAllWithDetail()
        {
            var o = ds.Albums.Include("Artirts").Include("Tracks").OrderBy(t => t.Name);

            return mapper.Map<IEnumerable<Album>, IEnumerable<AlbumWithDetails>>(o);
        }

        public AlbumWithDetails AlbumGetOne(int? id)
        {
            var o = ds.Albums.SingleOrDefault(t => t.Id == id);

            if ( o == null )
            {
                return null;
            }
            else
            {
                return mapper.Map<Album, AlbumWithDetails>(o);
            }
        }

        public AlbumBase AddAlbum(AlbumAdd newItem)
        {
            var ItemToBeAdded = ds.Albums.Add(mapper.Map<AlbumAdd, Album>(newItem));

            // fetch associated artist
            var o = ds.Artists.Find(newItem.ArtistId);

            if ( o == null )
            {
                return null;
            }

            // add artist to collection
            ItemToBeAdded.Artists.Add(o);

            //save user account
            ItemToBeAdded.Coordinator = UserAccount.Name;

            ds.SaveChanges();

            return mapper.Map<Album, AlbumBase>(ItemToBeAdded);
        }

        public bool DeleteAlbum(int id)
        {
            var ItemToDelete = ds.Albums.Find(id);

            // check if album is found
            if (ItemToDelete == null)
            { return false; }

            ds.Albums.Remove(ItemToDelete);

            ds.SaveChanges();

            return true;
        }
        // ####################################################################
        // ARTIST

        public IEnumerable<ArtistBase> ArtistGetAll()
        {
            return mapper.Map<IEnumerable<Artist>, IEnumerable<ArtistBase>>(ds.Artists.OrderBy(t => t.Name));
        }

        public IEnumerable<ArtistWithDetails> ArtistGetAllWithDetail()
        {
            var o = ds.Artists.Include("Albums").OrderBy(t => t.Name);

            return mapper.Map<IEnumerable<Artist>, IEnumerable<ArtistWithDetails>>(o);
        }

       public ArtistBase ArtistGetOne(int? id)
        {
            var o = ds.Artists.SingleOrDefault(t => t.Id == id);

            if (o == null)
            {
                return null;
            }
            else
            {
                return mapper.Map<Artist, ArtistBase>(o);
            }
        }

        public ArtistWithMedia ArtistGetOneWithMedia(int? id)
        {

            var o = ds.Artists.Include("ArtistMedia").SingleOrDefault(t => t.Id == id);

            if (o == null)
            {
                return null;
            }
            else
            {
                return mapper.Map<Artist, ArtistWithMedia>(o);
            }
        }
        public ArtistBase CreateArtist(ArtistAdd newItem)
        {
            var ItemToBeAdded = ds.Artists.Add(mapper.Map<ArtistAdd, Artist>(newItem));

            // set user account
            ItemToBeAdded.Executive = UserAccount.Name;

            //save
            ds.SaveChanges();

            return mapper.Map<Artist, ArtistBase>(ItemToBeAdded);

        }

        public bool DeleteArtist(int id)
        {
            var ItemToDelete = ds.Artists.Find(id);

            if (ItemToDelete == null)
            {
                return false;
            }

            ds.Artists.Remove(ItemToDelete);
            ds.SaveChanges();

            return true;
        }

        public ArtistWithMedia AddArtistMedia(ArtistMediaAdd newItem)
        {
            var a = ds.Artists.Find(newItem.ArtistId);

            if ( a == null )
            {
                return null;
            } 
            else
            {
                // attempt to add new item
                var AddedItem = new ArtistMedia();
                ds.ArtistsMedia.Add(AddedItem);

                AddedItem.Caption = newItem.Caption;
                AddedItem.Artist = a;

                // handle the uploaded photo

                //extract bytes
                byte[] MediaByte = new byte[newItem.MediaUpload.ContentLength];
                newItem.MediaUpload.InputStream.Read(MediaByte, 0, newItem.MediaUpload.ContentLength);

                // confugure new object
                AddedItem.content = MediaByte;
                AddedItem.ContentType = newItem.MediaUpload.ContentType;
                a.ArtistMedia.Add(AddedItem);

                ds.SaveChanges();

                // return artist, the media was created and stored in Database along with Artist Reference
                if (AddedItem == null)
                    return null;
                else
                    return mapper.Map<Artist, ArtistWithMedia>(a);

            }
        }

      /*  public  bool RemoveArtistMedia(ArtistWithMedia MyItem)
        {
            var Delete = ds.Artists.Include("ArtistMedia").SingleOrDefault(t => t.Id == MyItem.Id);

             if (Delete != null)
             {
                 var count = Delete.ArtistMedia.Count();
                 while (count > 0)
                 {
                     var a = Delete.ArtistMedia.First();
                     if ( a.Artist != null )
                     {
                         var b = ds.ArtistsMedia.Include("Artist").SingleOrDefault(m => m.Id == a.Artist.Id);
                         b.Artist = null;
                     }

                     Delete.ArtistMedia.Remove(a);                
                     count--;
                 }

       
            // Delete.ArtistMedia.Clear();
            // save changes after finished
            Delete.Profile = "This is Adele";
            ds.SaveChanges();
                return true;
            //}
            //else
              //  return false;
                    
        }*/


        // ##################################################################
        //ArtistMedia

            public ArtistMediaContent ArtistMediaGetByString(string stringId)
        {           
            var o = ds.ArtistsMedia.SingleOrDefault(m => m.StringId == stringId);

            return ( o == null ) ? null : mapper.Map<ArtistMedia, ArtistMediaContent>(o);

        }


        // ####################################################################
        // TRACK

        public IEnumerable<TrackBase> TrackGetAll()
        {
            return mapper.Map<IEnumerable<Track>, IEnumerable<TrackBase>>(ds.Tracks.OrderBy(t => t.Name));
        }

        public IEnumerable<TrackWithDetails> TrackGetAllWithDetail()
        {
            var o = ds.Tracks.Include("Albums").OrderBy(t => t.Name);

            return mapper.Map<IEnumerable<Track>, IEnumerable<TrackWithDetails>>(o);
        }

        public TrackWithDetails TrackGetOne(int? id)
        {
            var o = ds.Tracks.Include("Albums").SingleOrDefault(t => t.Id == id);

            if (o == null)
            {
                return null;
            }
            else
            {
                return mapper.Map<Track, TrackWithDetails>(o);
            }
        }

        public SampleClip SampleClipById(int id)
        {
            var o = ds.Tracks.Find(id);

            return (o == null) ? null : mapper.Map<Track, SampleClip>(o);

        }

        public TrackWithDetails AddTrack(TrackAdd newItem)
        {

            var ItemAdded = ds.Tracks.Add(mapper.Map<TrackAdd, Track>(newItem));

            if (ItemAdded == null)
            {
                return null;
            }
            else
            {
                // extract bytes from httppostedfile
                byte[] MediaBytes = new byte[newItem.SampleUpload.ContentLength];

                newItem.SampleUpload.InputStream.Read(MediaBytes, 0, newItem.SampleUpload.ContentLength);

                // configure new object properties
                ItemAdded.Media = MediaBytes;
                ItemAdded.MediaContentType = newItem.SampleUpload.ContentType;

                // add album reference to track
                var o = ds.Albums.Find(newItem.AlbumId);

                if ( o != null )
                     {
                          ItemAdded.Albums.Add(o);
                      }

                // add owner
                ItemAdded.Clerk = UserAccount.Name;

                ds.SaveChanges();

                return (ItemAdded == null) ? null : mapper.Map<Track, TrackWithDetails>(ItemAdded);
                

            }
        }
        // ##########################################################################
        // Genre

            public IEnumerable<GenreBase> GenreGetAll()
        {
            var o = ds.Genres;
            return mapper.Map<IEnumerable<Genre>, IEnumerable<GenreBase>>(o);
        }

        public bool LoadData()
        {
            // User name
            var user = HttpContext.Current.User.Identity.Name;

            // Monitor the progress
            bool done = false;

            // ############################################################
            // Genre

            if (ds.Genres.Count() == 0)
            {
                // Add genres

                ds.Genres.Add(new Genre { Name = "Alternative" });
                ds.Genres.Add(new Genre { Name = "Classical" });
                ds.Genres.Add(new Genre { Name = "Country" });
                ds.Genres.Add(new Genre { Name = "Easy Listening" });
                ds.Genres.Add(new Genre { Name = "Hip-Hop/Rap" });
                ds.Genres.Add(new Genre { Name = "Jazz" });
                ds.Genres.Add(new Genre { Name = "Pop" });
                ds.Genres.Add(new Genre { Name = "R&B" });
                ds.Genres.Add(new Genre { Name = "Rock" });
                ds.Genres.Add(new Genre { Name = "Soundtrack" });

                ds.SaveChanges();
                done = true;
            }

            // ############################################################
            // Artist

            if (ds.Artists.Count() == 0)
            {
                // Add artists

                ds.Artists.Add(new Artist
                {
                    Name = "The Beatles",
                    BirthOrStartDate = new DateTime(1962, 8, 15),
                    Executive = user,
                    Genre = "Pop",
                    UrlArtist = "https://upload.wikimedia.org/wikipedia/commons/9/9f/Beatles_ad_1965_just_the_beatles_crop.jpg"
                });

                ds.Artists.Add(new Artist
                {
                    Name = "Adele",
                    BirthName = "Adele Adkins",
                    BirthOrStartDate = new DateTime(1988, 5, 5),
                    Executive = user,
                    Genre = "Pop",
                    UrlArtist = "http://www.billboard.com/files/styles/article_main_image/public/media/Adele-2015-close-up-XL_Columbia-billboard-650.jpg"
                });

                ds.Artists.Add(new Artist
                {
                    Name = "Bryan Adams",
                    BirthOrStartDate = new DateTime(1959, 11, 5),
                    Executive = user,
                    Genre = "Rock",
                    UrlArtist = "https://upload.wikimedia.org/wikipedia/commons/7/7e/Bryan_Adams_Hamburg_MG_0631_flickr.jpg"
                });
              
                ds.SaveChanges();
                done = true;
            }

            // ############################################################
            // Album

            if (ds.Albums.Count() == 0)
            {
                // Add albums

                // For Bryan Adams
                var bryan = ds.Artists.SingleOrDefault(a => a.Name == "Bryan Adams");

                ds.Albums.Add(new Album
                {
                    Artists = new List<Artist> { bryan },
                    Name = "Reckless",
                    ReleaseDate = new DateTime(1984, 11, 5),
                    Coordinator = user,
                    Genre = "Rock",
                    UrlAlbum = "https://upload.wikimedia.org/wikipedia/en/5/56/Bryan_Adams_-_Reckless.jpg"
                });

                ds.Albums.Add(new Album
                {
                    Artists = new List<Artist> { bryan },
                    Name = "So Far So Good",
                    ReleaseDate = new DateTime(1993, 11, 2),
                    Coordinator = user,
                    Genre = "Rock",
                    UrlAlbum = "https://upload.wikimedia.org/wikipedia/pt/a/ab/So_Far_so_Good_capa.jpg"
                });

                ds.SaveChanges();
                done = true;
            }

            // ############################################################
            // Track

            if (ds.Tracks.Count() == 0)
            {
                // Add tracks

                // For Reckless
                var reck = ds.Albums.SingleOrDefault(a => a.Name == "Reckless");

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { reck },
                    Name = "Run To You",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { reck },
                    Name = "Heaven",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { reck },
                    Name = "Somebody",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { reck },
                    Name = "Summer of '69",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { reck },
                    Name = "Kids Wanna Rock",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                // For Reckless
                var so = ds.Albums.SingleOrDefault(a => a.Name == "So Far So Good");

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { so },
                    Name = "Straight from the Heart",
                    Composers = "Bryan Adams, Eric Kagna",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { so },
                    Name = "It's Only Love",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { so },
                    Name = "This Time",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { so },
                    Name = "(Everything I Do) I Do It for You",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.Tracks.Add(new Track
                {
                    Albums = new List<Album> { so },
                    Name = "Heat of the Night",
                    Composers = "Bryan Adams, Jim Vallance",
                    Clerk = user,
                    Genre = "Rock"
                });

                ds.SaveChanges();
                done = true;
            }

            // ############################################
            // Role Claims

            if (ds.RoleClaims.Count() == 0)
            {
                ds.RoleClaims.Add(new RoleClaim
                {
                    Name = "Executive"
                });
                ds.RoleClaims.Add(new RoleClaim
                {
                    Name = "Coordinator"
                });
                ds.RoleClaims.Add(new RoleClaim
                {
                    Name = "Clerk"
                });
                ds.RoleClaims.Add(new RoleClaim
                {
                    Name = "Staff"
                });
                ds.RoleClaims.Add(new RoleClaim
                {
                    Name = "General Manager"
                });
                ds.RoleClaims.Add(new RoleClaim
                {
                    Name = "Star Lord"
                });

                ds.SaveChanges();
                done = true;
            }
            return done;
        }
        public List<string> RoleCaimGetAllStrings()
        {
            // grab a collection of strings for each role claim
            var AllRoles = ds.RoleClaims.OrderBy(t => t.Name).Select(t => t.Name).ToList();

            return (AllRoles == null) ? null : AllRoles;
        }
        public bool RemoveData()
        {
            try
            {
                foreach (var e in ds.Tracks)
                {
                    ds.Entry(e).State = System.Data.Entity.EntityState.Deleted;
                }
                ds.SaveChanges();

                foreach (var e in ds.Albums)
                {
                    ds.Entry(e).State = System.Data.Entity.EntityState.Deleted;
                }
                ds.SaveChanges();

                foreach (var e in ds.Artists)
                {
                    ds.Entry(e).State = System.Data.Entity.EntityState.Deleted;
                }
                ds.SaveChanges();

                foreach (var e in ds.Genres)
                {
                    ds.Entry(e).State = System.Data.Entity.EntityState.Deleted;
                }
                ds.SaveChanges();

                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        public bool RemoveDatabase()
        {
            try
            {
                return ds.Database.Delete();
            }
            catch (Exception)
            {
                return false;
            }
        }

    }

    // New "UserAccount" class for the authenticated user
    // Includes many convenient members to make it easier to render user account info
    // Study the properties and methods, and think about how you could use it
    public class UserAccount
    {
        // Constructor, pass in the security principal
        public UserAccount(ClaimsPrincipal user)
        {
            if (HttpContext.Current.Request.IsAuthenticated)
            {
                Principal = user;

                // Extract the role claims
                RoleClaims = user.Claims.Where(c => c.Type == ClaimTypes.Role).Select(c => c.Value);

                // User name
                Name = user.Identity.Name;

                // Extract the given name(s); if null or empty, then set an initial value
                string gn = user.Claims.SingleOrDefault(c => c.Type == ClaimTypes.GivenName).Value;
                if (string.IsNullOrEmpty(gn)) { gn = "(empty given name)"; }
                GivenName = gn;

                // Extract the surname; if null or empty, then set an initial value
                string sn = user.Claims.SingleOrDefault(c => c.Type == ClaimTypes.Surname).Value;
                if (string.IsNullOrEmpty(sn)) { sn = "(empty surname)"; }
                Surname = sn;

                IsAuthenticated = true;
                IsAdmin = user.HasClaim(ClaimTypes.Role, "Admin") ? true : false;
            }
            else
            {
                RoleClaims = new List<string>();
                Name = "anonymous";
                GivenName = "Unauthenticated";
                Surname = "Anonymous";
                IsAuthenticated = false;
                IsAdmin = false;
            }

            NamesFirstLast = $"{GivenName} {Surname}";
            NamesLastFirst = $"{Surname}, {GivenName}";
        }

        // Public properties
        public ClaimsPrincipal Principal { get; private set; }
        public IEnumerable<string> RoleClaims { get; private set; }

        public string Name { get; set; }

        public string GivenName { get; private set; }
        public string Surname { get; private set; }

        public string NamesFirstLast { get; private set; }
        public string NamesLastFirst { get; private set; }

        public bool IsAuthenticated { get; private set; }

        // Add other role-checking properties here as needed
        public bool IsAdmin { get; private set; }

        public bool HasRoleClaim(string value)
        {
            if (!IsAuthenticated) { return false; }
            return Principal.HasClaim(ClaimTypes.Role, value) ? true : false;
        }

        public bool HasClaim(string type, string value)
        {
            if (!IsAuthenticated) { return false; }
            return Principal.HasClaim(type, value) ? true : false;
        }
    }

}