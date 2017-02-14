using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace Assignment9.Controllers
{
    public class ArtistMediaAddForm
    {
        public int ArtistId { get; set; }

        public string ArtistProperty { get; set; }

        // brief caption
        [Required, StringLength(100)]
        [Display(Name = "Descriptive Caption")]
        public string Caption { get; set; }

        [Required, Display(Name ="Media Item"), DataType(DataType.Upload)]
        public string MediaUpload { get; set; }
    }

    public class ArtistMediaAdd
    {
        public int ArtistId { get; set; }

        // brief caption
        [Required, StringLength(100)]
        [Display(Name = "Descriptive Caption")]
        public string Caption { get; set; }

        [Required]
        public HttpPostedFileBase MediaUpload { get; set; }
    }

    public class ArtistMediaBase 
    {

        public int Id { get; set; }

        [Display(Name = "Added on date/time")]
        public DateTime TimeStamp { get; set; }

        // for generated identifier
        [Required, StringLength(100)]
        [Display(Name = "Unique Identifier")]
        public string StringId { get; set; }

        // brief caption
        [Required, StringLength(100)]
        [Display(Name = "Descriptive Caption")]
        public string Caption { get; set; }

        public byte[] Content { get; set; }
        [StringLength(100)]

        public string ContentType { get; set; }
    }

    public class ArtistMediaContent
    {
        public int Id { get; set; }
        public byte[] Content { get; set; }
        [StringLength(100)]
        public string ContentType { get; set; }
    }
}
