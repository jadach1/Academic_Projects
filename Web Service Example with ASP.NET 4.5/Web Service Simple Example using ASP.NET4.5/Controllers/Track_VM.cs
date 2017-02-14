using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Mvc;

namespace Assignment9.Controllers
{
    public class TrackAdd
    {
        [Required, StringLength(200), Display(Name="Track name")]
        public string Name { get; set; }

        // Simple comma-separated string of all the track's composers
        [Required, StringLength(500), Display(Name = "Composer names (comma-seperated)")]
        public string Composers { get; set; }

        [Required, Display(Name = "Track genre")]
        public string Genre { get; set; }

        // User name who added/edited the track
        [StringLength(200)]
        public string Clerk { get; set; }      

        [StringLength(200), Display(Name = "Album Name")]
        public string AlbumName { get; set; }
      
        public int AlbumId { get; set; }

        [Required]
        public System.Web.HttpPostedFileBase SampleUpload { get; set; }
    }

    public class TrackAddForm : TrackAdd
    {
        public SelectList GenreList { get; set; }

        [Required, DataType(DataType.Upload), Display(Name = "Sample Clip")]
        public string SampleUpload { get; set; }
    }

    public class TrackBase : TrackAdd
    {
        [Key]
        public int Id { get; set; }
    }

    public class TrackWithDetails : TrackBase
    {
        public TrackWithDetails()
        {
            Albums = new List<AlbumBase>();
        }
        public IEnumerable<AlbumBase> Albums { get; set; }
    }

    public class SampleClip
    {
        public int Id { get; set; }

       [Display(Name = "Sample Clip")]
        public byte[] Media { get; set; }

        public string MediaContentType { get; set; }
    }
}
